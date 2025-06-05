using System;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ooadepazar.Migrations
{
    /// <inheritdoc />
    public partial class datumazuriranjauartiklu : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AddColumn<DateTime>(
                name: "DatumAzuriranja",
                table: "Artikal",
                type: "datetime2",
                nullable: false,
                defaultValue: new DateTime(1, 1, 1, 0, 0, 0, 0, DateTimeKind.Unspecified));
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "DatumAzuriranja",
                table: "Artikal");
        }
    }
}
